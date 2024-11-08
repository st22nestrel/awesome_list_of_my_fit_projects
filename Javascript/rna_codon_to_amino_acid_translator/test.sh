#Author: Timotej Ponek, xponek00, VUT FIT Brno
#!/bin/bash

if [[ $1 == "--install" ]]; then
    # install dependencies
    npm install --save-dev uvu
    exit 0
fi

node translation.tests.mjs
