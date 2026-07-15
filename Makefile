# 可以更改$(COMPILER)以及$(REMOVE)来自定义编译环境
COMPILER = g++
STD = c++20

BASIC_LINK =	-I config

TRAIT_LINK =	-I include/ArrayList/trait \
				-I include/String/trait \
				-I include/HashMap/trait \
				-I include/SmartPtr/trait

STDCPP_LINK =	-I include/Basic \
				-I include/ArrayList/stdcpp \
				-I include/String/stdcpp \
				-I include/HashMap/stdcpp \
				-I include/HeapAllocator/stdcpp \
				-I include/AllocatorAdapter/stdcpp \
				-I include/SmartPtr/stdcpp \
				-lm

LINK ?= 		$(BASIC_LINK) \
				$(TRAIT_LINK) \
				$(STDCPP_LINK)

TEST_LIST =	String/UnitTest.cpp \
			ArrayList/UnitTest.cpp \
			HashMap/UnitTest.cpp \
			SmartPtr/UnitTest.cpp

TEST_TARGETS = $(addprefix test_,$(TEST_LIST))

.PHONY: test $(TEST_TARGETS)

test: $(TEST_TARGETS)

$(TEST_TARGETS): test_%:
	$(COMPILER) test/$* \
	-o bin/stamon_test \
	-O0 -g \
	-std=$(STD) \
	-static \
	$(LINK) \
	-I test/doctest

	-./bin/stamon_test

# CI Test 测试
TEST_XML_DIR = bin/test_ci_result

TEST_CI_TARGETS = $(addprefix test_ci_,$(TEST_LIST))

.PHONY: test_ci $(TEST_CI_TARGETS)

test_ci: $(TEST_CI_TARGETS)

$(TEST_CI_TARGETS): test_ci_%:
	mkdir -p $(TEST_XML_DIR)
	$(COMPILER) test/$* \
	-o bin/stamon_test \
	-O0 -g \
	-std=$(STD) \
	-static \
	$(LINK) \
	-I test/doctest

	./bin/stamon_test \
		--reporters=junit \
		--out=$(TEST_XML_DIR)/$(subst /,_,$*).xml \
		--no-colors=true