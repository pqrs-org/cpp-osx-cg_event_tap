#include <iostream>
#include <pqrs/cf/cf_ptr.hpp>
#include <pqrs/osx/cg_event_tap.hpp>

namespace {
struct callback_context final {
  pqrs::osx::cg_event_tap* event_tap = nullptr;
};

CGEventRef callback(CGEventTapProxy,
                    CGEventType type,
                    CGEventRef event,
                    void* refcon) {
  auto context = static_cast<callback_context*>(refcon);

  if (type == kCGEventTapDisabledByTimeout ||
      type == kCGEventTapDisabledByUserInput) {
    if (context &&
        context->event_tap) {
      if (!context->event_tap->enable()) {
        std::cerr << "Failed to re-enable the event tap." << std::endl;
      }
    }
    return event;
  }

  std::cout << "key event: " << type << std::endl;
  return event;
}
} // namespace

int main() {
  callback_context context;
  auto port = pqrs::cf::adopt_cf_ptr(CGEventTapCreate(kCGSessionEventTap,
                                                      kCGHeadInsertEventTap,
                                                      kCGEventTapOptionListenOnly,
                                                      CGEventMaskBit(kCGEventKeyDown) |
                                                          CGEventMaskBit(kCGEventKeyUp),
                                                      callback,
                                                      &context));
  if (!port) {
    std::cerr << "Failed to create an event tap. Grant Accessibility permission to this application."
              << std::endl;
    return 1;
  }
  pqrs::osx::cg_event_tap event_tap(std::move(port));
  context.event_tap = &event_tap;
  pqrs::cf::cf_ptr<CFRunLoopRef> run_loop(CFRunLoopGetCurrent());
  if (!event_tap.attach_to_run_loop(run_loop) ||
      !event_tap.enable()) {
    std::cerr << "Failed to start the event tap." << std::endl;
    return 1;
  }

  std::cout << "Listening for key events. Press Control-C to stop." << std::endl;
  CFRunLoopRun();
}
