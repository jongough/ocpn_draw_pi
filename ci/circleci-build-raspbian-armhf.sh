#!/usr/bin/env bash

#
# Build for Raspbian in a docker container
#

# bailout on errors and echo commands.
set -xe

DOCKER_SOCK="unix:///var/run/docker.sock"

echo "DOCKER_OPTS=\"-H tcp://127.0.0.1:2375 -H $DOCKER_SOCK -s devicemapper\"" | sudo tee /etc/default/docker > /dev/null
sudo service docker restart
sleep 5;

if [ "$BUILD_ENV" = "raspbian" ]; then
    docker run --rm --privileged multiarch/qemu-user-static:register --reset
else
    docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
fi

docker run --privileged -d -ti -e "container=docker"  -v $(pwd):/ci-source:rw $DOCKER_IMAGE /bin/bash
#DOCKER_CONTAINER_ID=$(docker ps | grep $BUILD_ENV | awk '{print $1}')
DOCKER_CONTAINER_ID=$(docker ps | awk '/balenalib/ {print $1}')

#docker exec -ti $DOCKER_CONTAINER_ID sudo curl http://mirrordirector.raspbian.org/raspbian.public.key  | sudo apt-key add -
#docker exec -ti $DOCKER_CONTAINER_ID sudo curl http://archive.raspbian.org/raspbian.public.key  | sudo apt-key add -

#docker exec -ti $DOCKER_CONTAINER_ID sudo apt-get update
#docker exec -ti $DOCKER_CONTAINER_ID sudo apt-get -y install git cmake build-essential cmake gettext wx-common libgtk2.0-dev libwxgtk3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release


echo $OCPN_TARGET
docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    -e "CIRCLECI=$CIRCLECI" \
    -e "CIRCLE_BRANCH=$CIRCLE_BRANCH" \
    -e "CIRCLE_TAG=$CIRCLE_TAG" \
    -e "CIRCLE_PROJECT_USERNAME=$CIRCLE_PROJECT_USERNAME" \
    -e "CIRCLE_PROJECT_REPONAME=$CIRCLE_PROJECT_REPONAME" \
    -e "GIT_REPOSITORY_SERVER=$GIT_REPOSITORY_SERVER" \
    -e "OCPN_TARGET=$OCPN_TARGET" \
    -e "TRAVIS=$TRAVIS" \
    -e "TRAVIS_REPO_SLUG=$TRAVIS_REPO_SLUG" \
    -e "TRAVIS_BRANCH=$TRAVIS_BRANCH" \
    -e "TRAVIS_TAG=$TRAVIS_TAG"

# Run build script
cat > build.sh << "EOF"
curl http://mirrordirector.raspbian.org/raspbian.public.key  > raspikey
sudo apt-key add raspikey
curl http://archive.raspbian.org/raspbian.public.key  > raspikey
sudo apt-key add raspikey
sudo apt -q update
sudo apt-get -y install --no-install-recommends \
    git cmake build-essential cmake gettext wx-common libgtk2.0-dev libwxgtk3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release
EOF

docker exec -ti \
    $DOCKER_CONTAINER_ID /bin/bash -xec "bash -xe /ci-source/build.sh; rm -rf ci-source/build; mkdir ci-source/build; cd ci-source/build; cmake ..; make $BUILD_FLAGS; make package; chmod -R a+rw ../build;"

echo "Stopping"
docker ps -a
docker stop $DOCKER_CONTAINER_ID
docker rm -v $DOCKER_CONTAINER_ID

