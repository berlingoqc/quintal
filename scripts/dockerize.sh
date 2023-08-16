#! /bin/bash

(cd ui && npm run docker:build && npm run docker:publish)
(cd backend && npm run docker:build && npm run docker:publish)
