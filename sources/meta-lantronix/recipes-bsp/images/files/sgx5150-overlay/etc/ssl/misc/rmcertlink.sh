#!/bin/sh
# rmcertlink.sh filename [filename ...]

cd /etc/ssl/certs

for CERTFILE in $*; do
  # Check each file in /etc/ssl/certs
  for file in $(ls *); do
    # Is the file a symlink?
    if [ -h $file ] ; then
      # Determine if the symlink points to our cert
      TARGET_PATH=$(readlink $file)
      if [ "$CERTFILE" = "$TARGET_PATH" ]; then
        rm -f $file
      fi
    fi
  done
done

#update the combined (trusted+intermediate) PEM file
find /etc/ssl/certs/ -name '*auth*.pem' | xargs cat > /etc/ssl/certs/combined.pem
#bundle this with the mach10.crt
cat /etc/ssl/certs/combined.pem > /etc/ssl/certs/mach10-combined.crt
cat /etc/ssl/certs/mach10.crt >> /etc/ssl/certs/mach10-combined.crt
exit 0

