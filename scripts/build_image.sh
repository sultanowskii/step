#!/bin/bash

docker build --tag 'step-valgrind' -f scripts/valgrind.Dockerfile .
