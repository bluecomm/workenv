sed -r "s:\x1B\[[0-9;]*[mK]::g" input0.txt
sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[m|K]//g" input0.txt
