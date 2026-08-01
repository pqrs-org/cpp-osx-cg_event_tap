#include <boost/ut.hpp>
#include <pqrs/cf/cf_ptr.hpp>
#include <pqrs/osx/cg_event_tap.hpp>
#include <type_traits>
#include <utility>

namespace {
void callback(CFMachPortRef,
              void*,
              CFIndex,
              void*) {
}
} // namespace

int main() {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "type properties"_test = [] {
    expect(std::is_constructible_v<pqrs::osx::cg_event_tap,
                                   pqrs::cf::cf_ptr<CFMachPortRef>&&>);
    expect(!std::is_copy_constructible_v<pqrs::osx::cg_event_tap>);
    expect(!std::is_copy_assignable_v<pqrs::osx::cg_event_tap>);
  };

  "empty port"_test = [] {
    pqrs::osx::cg_event_tap event_tap(
        pqrs::cf::cf_ptr<CFMachPortRef>{});
    pqrs::cf::cf_ptr<CFRunLoopRef> run_loop(CFRunLoopGetCurrent());

    expect(event_tap.valid() == false);
    expect(event_tap.attach_to_run_loop(run_loop) == false);
    expect(event_tap.enable() == false);
    expect(event_tap.disable() == false);

    event_tap.invalidate();
    expect(event_tap.valid() == false);
  };

  "empty run loop"_test = [] {
    auto port = pqrs::cf::adopt_cf_ptr(CFMachPortCreate(kCFAllocatorDefault,
                                                        callback,
                                                        nullptr,
                                                        nullptr));
    pqrs::osx::cg_event_tap event_tap(std::move(port));

    expect(event_tap.attach_to_run_loop(
               pqrs::cf::cf_ptr<CFRunLoopRef>{}) == false);
    expect(event_tap.valid() == true);
  };

  "attach and invalidate"_test = [] {
    auto port = pqrs::cf::adopt_cf_ptr(CFMachPortCreate(kCFAllocatorDefault,
                                                        callback,
                                                        nullptr,
                                                        nullptr));
    expect(static_cast<bool>(port));

    pqrs::osx::cg_event_tap event_tap(std::move(port));
    pqrs::cf::cf_ptr<CFRunLoopRef> run_loop(CFRunLoopGetCurrent());
    expect(event_tap.valid() == true);
    expect(event_tap.attach_to_run_loop(run_loop) == true);
    expect(event_tap.attach_to_run_loop(run_loop) == false);

    event_tap.invalidate();
    expect(event_tap.valid() == false);

    // invalidate is intentionally idempotent.
    event_tap.invalidate();
    expect(event_tap.valid() == false);
  };

  return 0;
}
