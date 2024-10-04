# PowerShell script to install Rust, Python, and pip

# Install Rust
Write-Host "Installing Rust..."
Invoke-WebRequest -Uri https://sh.rustup.rs -OutFile rustup-init.exe
Start-Process -FilePath "./rustup-init.exe" -ArgumentList "-y" -NoNewWindow -Wait
Remove-Item -Path "./rustup-init.exe"
$env:PATH += ";$($env:USERPROFILE)\.cargo\bin"
rustup update
Write-Host "Rust installed successfully."

# Install Python (Using the Windows Package Manager if available)
if (Get-Command winget -ErrorAction SilentlyContinue) {
    Write-Host "Installing Python using winget..."
    winget install -e --id Python.Python.3
} else {
    Write-Host "Installing Python using the official installer..."
    $pythonInstallerUrl = "https://www.python.org/ftp/python/3.10.10/python-3.10.10-amd64.exe"  # You can update this to the latest version
    $pythonInstallerPath = "$env:TEMP\python-installer.exe"
    Invoke-WebRequest -Uri $pythonInstallerUrl -OutFile $pythonInstallerPath
    Start-Process -FilePath $pythonInstallerPath -ArgumentList "/quiet InstallAllUsers=1 PrependPath=1" -NoNewWindow -Wait
    Remove-Item $pythonInstallerPath
}
$env:PATH += ";$($env:ProgramFiles)\Python310"

# Verify Python installation
Write-Host "Verifying Python installation..."
python --version

# Install pip (if not already included with Python)
Write-Host "Installing/upgrading pip..."
python -m ensurepip --upgrade

# Verify pip installation
Write-Host "Verifying pip installation..."
pip --version

Write-Host "Installing packages"
pip install -r requirements.txt

Write-Host "All installations completed successfully."
