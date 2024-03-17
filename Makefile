PROJECT_ROOT_PATH = $(shell pwd)

all:
	# echo $(shell pwd)
	cd $(PROJECT_ROOT_PATH)/server && $(MAKE) all
	cd $(PROJECT_ROOT_PATH)/client && $(MAKE) all 
	
.PHONY: clean
clean:
	cd "./client/" && $(MAKE) clean
	cd "./server/" && $(MAKE) clean

export PROJECT_ROOT_PATH