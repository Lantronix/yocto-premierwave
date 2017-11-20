#!/bin/sh
#normalize newline characters
dos2unix $1
cert_count=0
rm -f $1_*
while IFS=$'\n' read -r l || [[ -n "$l" ]]; do
    if [ "$l" == "-----BEGIN CERTIFICATE-----" ]; then
        rm -f $1_$cert_count
    fi
    echo $l >> $1_$cert_count
    if [ "$l" == "-----END CERTIFICATE-----" ]; then
        cert_count=$((cert_count + 1))
    fi
done < $1
echo "$cert_count"
