python3 -m maturin build --release

Get-ChildItem .\target\wheels\*.whl | ForEach-Object { python3 -m pip install --force-reinstall $_.FullName }
