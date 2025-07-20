# This is repo for udemy cpp tasks and exercies
# v 0.02

# vs_code launch.json contents for lldb debug

```bash
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "type": "lldb",
            "request": "launch",
            "name": "Launch",
            "program": "${workspaceFolder}/build/udemy_1_initial",
            "args": [],
            "cwd": "${workspaceFolder}"
        }
    ]
}


mmcli -m 0 --command=AT+CFUN=1


mmcli -m 0 -d
mmcli -m 0 --set-power-state-on //  Set full power state in the modem
mmcli -m 0 -e

And then restart your device

APN: internet.a1.bg


nmcli connection add type gsm ifname ttyUSB3 con-name 5g
nmcli connection modify id "5g" gsm.apn "internet.a1.bg"

nmcli connection up id "5g"

nmcli connection show 5g

mmcli -m 3 --command=AT+CGDATA="PPP",internet.a1.bg


sudo rm /etc/NetworkManager/system-connections/5g
sudo systemctl restart NetworkManager

mmcli -m 0 --command=AT+CGDCONT?

```
