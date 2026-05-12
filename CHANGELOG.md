# CHANGELOG

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased] 

## [0.9.0] - 2026-5-15
### Added

- Introduced this Changelog
- Preparations for releasing a 1.0 version that can be registered with the Arduino
Library manager and so more readily used by interested projects.


### Changed
- Simplifed the large switch statement in `update()` to avoid compilation errors with some IDEs and platforms. More complex mode updates are now handled through calls to separate (private) mode-specific update functions. This should also facilitate adding support in the future for additional display modes.
- Renamed default branch to 'main'

## [0.5.0] - 2019-11-02

### Added
- Initial packaging as an Arduino Library
- Included two sample applications
