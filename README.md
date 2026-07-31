[![Build Status](https://github.com/pqrs-org/cpp-osx-cg_event_tap/workflows/CI/badge.svg)](https://github.com/pqrs-org/cpp-osx-cg_event_tap/actions)
[![License](https://img.shields.io/badge/license-Boost%20Software%20License-blue.svg)](https://github.com/pqrs-org/cpp-osx-cg_event_tap/blob/main/LICENSE.md)

# cpp-osx-cg_event_tap

A small C++ wrapper for managing a macOS `CGEventTap`.

`pqrs::osx::cg_event_tap` takes ownership of the `CFMachPortRef` returned by
`CGEventTapCreate`, attaches it to a `CFRunLoop`, and provides methods for
enabling, disabling, and invalidating the event tap. The event tap, run loop
source, and retained run loop reference are released automatically when the
wrapper is destroyed.

## Requirements

cpp-osx-cg_event_tap depends the following classes.

- [pqrs::cf::cf_ptr](https://github.com/pqrs-org/cpp-cf-cf_ptr).

## Install

Copy `include/pqrs` and `vendor/vendor/include` directories into your include directory.
