#!/bin/sh
# certlink.sh filename [filename ...]

for CERTFILE in $*; do
  # make sure file exists and is a valid cert
  test -f "$CERTFILE" || continue
  HASH=$(openssl x509 -noout -hash -in "$CERTFILE")
  HASHBASE=/etc/ssl/certs/"$HASH"
  test -n "$HASH" || continue

  # use lowest available iterator for symlink
  for ITER in 0 1 2 3 4 5 6 7 8 9; do
    test -f "${HASHBASE}.${ITER}" && continue
    ln -s "$CERTFILE" "${HASHBASE}.${ITER}"
    test -L "${HASHBASE}.${ITER}" && break
  done
done

#update the combined (trusted+intermediate) PEM file
find /etc/ssl/certs/ -name '*auth*.pem' | xargs cat > /etc/ssl/certs/combined.pem
#bundle this with the mach10.crt
cat /etc/ssl/certs/combined.pem > /etc/ssl/certs/mach10-combined.crt
cat /etc/ssl/certs/mach10.crt >> /etc/ssl/certs/mach10-combined.crt
exit 0

