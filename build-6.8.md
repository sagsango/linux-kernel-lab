One time Setup
# 1. Create Dockerfile
	Dockerfile is already here
# 2. Build the docker image
	docker build -t kernel-builder .


Deply the build envirnmnet:
sudo docker run -it --rm -v "$PWD":/build kernel-builder bash
