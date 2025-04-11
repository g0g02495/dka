# Disk-Keep-Alive

## Description
This program is a simple keep-alive script for a disk drive. It checks if a specified directory exists and then enters<br/>
a loop that runs a command to keep the drive active. The command used is 'dd' which reads a block of data from the drive and<br/>
discards it, preventing the drive from going to sleep. The program takes two arguments: the path to the directory and an optional<br/>
wait time in seconds (default value is 300).

## Compilation & Installation
Compile simply with <code>gcc dka.c -o dka</code> and copy executable into folder located on your PATH, e.g. <code>/usr/bin</code>

## Usage
The usage is:
<code>
dka </path/to/disk-device> [wait_seconds]
</code>

## Set Up as Service
You can register the utility with systemd as a service. The device path can be specifed in service descriptor as shown below.<br/>
Do not specify User in the Service section, as the dd command needs to be executed by root.

### Note:
Specify the disk target based on its **UUID**, because the usual way **/dev/sdx** target may change after reboot.

Create service descriptor file in **/etc/systemd/system/dka.service** and register it with **systemd**:
<code><br/>sudo systemct enable dka.service</code>

<code>
[Unit]<br/>
Description=Disk-Keep-Alive service<br/>
After=local-fs.target<br/>
StartLimitBurst=5<br/>
StartLimitIntervalSec=60<br/><br/>

[Service]<br/>
Type=simple</br>
Restart=always<br/>
RestartSec=30<br/>
Environment=DKA_TARGET=/dev/disk/by-uuid/86F067E0F067D4C9<br/>
ExecStart=/usr/bin dka ${DKA_TARGET} 300<br/><br/>
[Install]<br/>
WantedBy=multi-user.target<br/>
</code>
