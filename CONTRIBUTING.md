# Contributing to FairCom FDW for PostgreSQL

Thank you for considering contributing to this project! We welcome contributions from the community.

## How to Contribute

### Reporting Bugs
- Use GitHub Issues to report bugs
- Include PostgreSQL version, OS, and FDW version
- Provide steps to reproduce the issue
- Include relevant logs with `SET client_min_messages = DEBUG1;`

### Suggesting Features
- Open a GitHub Discussion to propose new features
- Explain the use case and expected behavior
- Consider backward compatibility

### Submitting Pull Requests
1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature-name`
3. Make your changes in the `core/src` directory
4. Add tests in the `tests/` directory
5. Ensure all tests pass: `python3 tests/test_fdw.py`
6. Commit with clear messages: `git commit -m "Add feature: description"`
7. Push to your fork: `git push origin feature/your-feature-name`
8. Open a Pull Request with a clear description

### Code Style
- Follow PostgreSQL C coding conventions
- Use 4-space indentation
- Add comments for non-obvious logic
- Keep functions focused and under 100 lines when possible
- Avoid trailing whitespace; verify with `git diff --check`
- If you have the pre-commit hook installed, it will block commits with trailing whitespace

### Testing
- All new features must include tests
- Run the full test suite before submitting
- Test on both Debian and RPM platforms when possible

### Development Environment
See the [Docker Quick Start](README.md#try-it-with-docker) section of the README for setting up your development environment.

## Code of Conduct
Be respectful and constructive in all interactions. We aim to foster an inclusive community.

## Questions?
Open a GitHub Discussion or reach out via the support channels listed in the README.
