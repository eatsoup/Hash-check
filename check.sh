#!/bin/bash
# Simple bash script for automating password hash check
# Requirements: Python

# Fetch user from commandline
user=$1
if [ x$user == x ]
then
    echo Please supply username
    exit 1
fi

# Read password from STDIN
read -sp 'Password:' password_input
# Save input password in env for use in Python
export password=$password_input
encryptionType=$(cat /etc/shadow | grep ^$user | cut -d '$' -f 2)
salt=$(cat /etc/shadow | grep ^$user | cut -d '$' -f 3)
ourhash=$(cat /etc/shadow | grep ^$user | cut -d '$' -f 4 | cut -d ':' -f 1)

# Save password hash in env for use in Python
export concat=$(sed 's/\$/\$/g' <<< \$$encryptionType\$$salt)
temphash=$(python -c "import crypt, os; print(crypt.crypt(os.environ.get('password'), os.environ.get('concat')))")
theirhash=$(echo $temphash | cut -d '$' -f 4 | cut -d ':' -f1)
echo

# Compare hashes
if [ $ourhash == $theirhash ]
then
    echo "PASSWORD MATCHES"
else
    echo "PASSWORD INCORRECT"
fi
unset password
