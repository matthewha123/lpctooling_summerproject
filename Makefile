SRC_DIRS = $(shell find src -maxdepth 1 -mindepth 1 -type d)
ALL_BOARDS = $(patsubst src/%,%, $(SRC_DIRS))

CANSPEC_NAME = can_spec_my18.yml
CANLIB_PATH = ./lib/CANlib
NON_CAN_TARGETS = $(CANLIB_PATH)/can_raw.h $(CANLIB_PATH)/static_can.h $(CANLIB_PATH)/evil_macros.h
CAN_TARGETS=$(filter-out $(NON_CAN_TARGETS), $(wildcard $(CANLIB_PATH)/*c $(CANLIB_PATH)/*h))

all: prepare boards

prepare:
	@echo "Running all preparatory steps..."
	@(for x in scripts/prepare/*.sh; do ./$$x && echo "$$x DONE!\n" || echo "$$x FAILED!\n" ; done)

pull:
	scripts/pull.sh

upgrade:
	scripts/upgrade.sh

toolchain:
	scripts/toolchain/all.sh

program:
	$(eval MODE+=writeflash)
	@echo 'If BOARDS env variable is not defined, provide a board name as a target'

ifdef BOARDS
	$(MAKE) $(BOARDS) MODE=writeflash
endif

com:
	$(eval MODE+=com)
	@echo 'If BOARDS env variable is not defined, provide a board name as a target'
ifdef BOARDS
	$(MAKE) $(BOARDS) MODE=com
endif

boards:
ifdef BOARDS
	$(MAKE) $(BOARDS)
else
	$(MAKE) $(ALL_BOARDS)
endif

$(ALL_BOARDS): canlib
	@echo 'Running make with mode: '$(MODE)
	@echo 'BOARDS env var is: '$(BOARDS)
	for mode in $(MODE)' '; do \
		make -C src/$@ $$mode -e;\
	done; \

canlib: $(CAN_TARGETS)

$(CAN_TARGETS): $(CANSPEC_NAME)
	scripts/prepare/canlib.sh

.PHONY: all prepare boards canlib$(BOARDS)