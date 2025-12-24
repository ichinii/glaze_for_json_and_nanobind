## using glaze to auto-generate json and nanobind bindings

### usage

```bash
./compile_and_test.sh
```

## TODO

### generating stub files
in order to generate IDE-visible annotations and static analysis support, you can use the `nanobind.stubgen` module to create a stub file for your nanobind module.
to generate the stub file `test_py.pyi` for the module `test_py`, run the following command:
```bash
python -m nanobind.stubgen -m test_py
```
NOTE: we should find a way to re-use the docstrings from the C++ code in the generated stubs, using a preprocessing step or similar. (https://www.perplexity.ai/search/how-do-i-provide-documentation-87_ewJLHQ8WEKzQ_1pNmgw#2)
