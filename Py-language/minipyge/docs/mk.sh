set -e

sphinx-apidoc -o modules/ ..
make html
cp -r _build/html/* /var/www/html/minipyge/
