09/22/2023 18:07:02
# Testing Overview

This is a document on how to use our test setup. We've decided to use [k6](https://k6.io/) [`0.46`](https://github.com/grafana/k6/releases/tag/v0.46.0)  for testing. This is because it allows for simple scripts to be set-up and tested. It also allows for disrupted or faulty HTTP request.

---
### Instalation 
We've put the binary in `bin/` for ease of use. So when running 

