# Contributing Guide [Eng]

First of all, thank you for your interest in the project!

## First steps

- To contribute, you need to fork the central repository.
  - Remember to stay up-to-date with central repository. This will minimise merge conflicts
- Clone your fork on your local machine using `git clone https://github.com/your-username/AxonEngine-fork`
- [Configure your local repository](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/configuring-a-remote-repository-for-a-fork)
- - `git remote add origin <YOUR FORK>`
  - `git remote add upstream <AXON-CENTRAL>`
- After you push some changes to your fork, open a pull-request to a new branch on central repository
- If your branch passes status checks and code review, it will be merged to `main`

## Branch naming tutorial

I have `pull-request-checker.yaml` setted up in GitHub Actions. This action checks branch naming for all pull-requests, targeted to `main` and `feature/*` branches. So, please foolow branch naming rules, otherwise your pull-request will be declined.

- Every work-in-progress branch should start with `wip/`
- Central section should representate, which region of code you're currently changing. It can be `actions/`, `server/`, `client/`, `global/` if you're working on both server and client, `docs/`, etc.
- The last section is your brief message, e.g. `global-unix-backend`

## Building and testing

> Building tutorial will be written in GitHub Wiki

Status checks require your code to build successfully and pass all unit tests. You can try this locally by running `cmake` and `ctest` in your project directory.

> Please note that tests run on different configurations, including x86 and x64 architectures. Failed tests won't let your changes merge

## Codestyle

> This section is in progress
