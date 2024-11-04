# Python virtual environment setup manual
> [!NOTE]
> It is not necessary to use python3.8, you can use whichever version you want. However, most of the projects were developed and tested with python3.8, that's why it is included in all manuals and readmes. It is also useful to specify exact version of python you want to use in the cmd if you have multiple python versions installed (which is very likely)

1. Create venv
    ```bash
    python3.8 -m venv venv
    ```
2. Activate venv
    ```bash
    source venv/bin/activate
    ```
3. Install requirements
    ```bash
    pip3.8 install -r requirements.txt
    ```

To deactivate venv, just run `deactivate` in shell.