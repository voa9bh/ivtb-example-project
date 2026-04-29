# This Dockerfile is used to build an image containing basic stuff to be used as a Jenkins slave build node.

# local Test
# navigate to dir with Dockerfile
# BUILD DockerContainer
# sudo docker build -t gradle-5-6-3-jdk8 .

# INSPECT Docker if Updates needed before RUN
# docker inspect -f . gradle-5-6-3-jdk8

# RUN DockerContainer
# sudo docker run -i -w /var/lib/jenkins/workspace -v $(pwd):/var/lib/jenkins/workspace:rw,z -t gradle-5-6-3-jdk8 /bin/bash
# export GRADLE_OPTS="-Dhttps.proxyHost=172.17.0.1 -Dhttps.proxyPort=3129 -Dorg.gradle.java.home=/opt/java/openjdk"
# chmod +x gradlew
# ./gradlew jar or gradle jar
# ./gradlew check or gradle check
# exit

FROM gradle:5.6.2-jdk8

ENV http_proxy http://172.17.0.1:3129
ENV https_proxy http://172.17.0.1:3129
RUN export http_proxy='http://172.17.0.1:3129'
RUN export https_proxy='http://172.17.0.1:3129'
RUN echo http proxy: ${http_proxy} ${HTTP_PROXY}
RUN echo https proxy: ${https_proxy} ${HTTPS_PROXY}

# Install dependencies
RUN apt-get update -y
RUN apt-get install -y \
    zip \
    unzip \
    git \
    wget
