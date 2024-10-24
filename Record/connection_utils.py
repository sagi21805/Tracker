import os
from pathlib import Path
from time import sleep
import subprocess

class RemoteHost:

    def __init__(self, ssh_cred: str):
        self.username, self.host = ssh_cred.split("@")
        self.ssh_cred = ssh_cred
        self.home = f"/home/{self.username}"

    def run_command(self, command):
        os.system(f"ssh {self.ssh_cred} {command}")

    def run_python_script(self, script_path: Path) -> str:
        os.system(f"ssh {self.ssh_cred} 'python3 {script_path.resolve()}' &")
        sleep(1)
        return subprocess.check_output(
            f"ssh {self.ssh_cred} 'pgrep -f {script_path.resolve()}'", shell=True
            ).decode('utf-8').strip()
    
    def transfer_files(self, src_path: Path, dst_path: Path):
        os.system(f"scp {self.ssh_cred}:{src_path} {dst_path}")

    def kill_process(self, pid):
        os.system(f"ssh {self.ssh_cred} kill -9 {pid}")

    def remove_file(self, file_path: Path):
        os.system(f"ssh {self.ssh_cred} rm {file_path.resolve()}")

class 