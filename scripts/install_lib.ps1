python -m maturin build --release
python -m pip install target/wheels/* --force-reinstall