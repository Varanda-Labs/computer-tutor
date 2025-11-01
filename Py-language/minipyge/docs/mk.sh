set -e
rm -rf _build/* || true
rm -rf modules/* || true
sphinx-apidoc -o modules/ ..
make html
cp -r _build/html/* /var/www/html/minipyge/
