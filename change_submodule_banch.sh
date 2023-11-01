#!/usr/bin/env bash

if [ "$#" -ne "2" ]; then
    echo "Incorrect invocation: Should be change_submodule_branch.sh branch git-url"
    echo "where:"
    echo "  branch is the new branch required"
    echo "  git-url is the url of the submodule required"
    echo "  example:"
    echo "  change_submodule_branch main https://github.com/OpenCPN/opencpn-libs.git"
    exit
fi

git submodule deinit -f opencpn-libs
git rm --cached opencpn-libs
rm -rf .git/modules/opencpn-libs  #(on Windows, simply delete the target directory ".git/modules/opencpn-libs" )
rm -rf opencpn-libs  #(on Windows, simply delete the target directory "opencpn-libs" )
git config -f .gitmodules --remove-section submodule.opencpn-libs
git add .gitmodules
git commit -m "Remove opencpn-libs submodule."

git submodule add -b $1 $2
git commit -m "Adding $2 submodule branch $1"

echo "This will now need to be pushed to make it available to circleci builds in the cloud"
