CLEANUP = rm -f
MKDIR = mkdir -p
TARGET_EXTENSION  = exe

.PHONY: clean
.PHONY: test

#Set directory paths
PATHU = unity/src/
PATHS = src/
PATHT = test/
PATHI = inc/

PATHB = build/
PATHD = build/depends/
PATHO = build/objs/
PATHR = build/results/

BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)

SRCT = $(wildcard $(PATHT)*.c)

COMPILE = gcc -c
LINK = gcc
DEPEND = gcc -MM -MG -MF
CFLAGS = -I. -I$(PATHU) -I$(PATHS) -DTEST

RESULTS = $(patsubst $(PATHT)Test%.c,$(PATHR)Test%.txt,$(SRCT))

#Search results directory files for lines 
#that contain PASS, FAIL, or IGNORE store
#them in respective variable
#Suppress error messages about unreadable files

PASSED = `grep -s PASS $(PATHR)*.txt`
FAIL = `grep -s FAIL $(PATHR)*.txt`
IGNORE = `grep -s IGNORE $(PATHR)*.txt`

#Echo result messages
test: $(BUILD_PATHS) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

#Run executable and redirect stdout and std err to results text file
$(PATHR)%.txt: $(PATHB)%.$(TARGET_EXTENSION)
	-./$< > $@ 2>&1

#Link All object files and output to Test executable
$(PATHB)Test%.$(TARGET_EXTENSION): $(PATHO)Test%.o $(PATHO)%.o $(PATHU)unity.o #$(PATHD)Test%.d
	$(LINK) -o $@ $^

#Compile test files and store the resultant object files in object folder
$(PATHO)%.o:: $(PATHT)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

#Compile source files and store the resultant object files in object folder
$(PATHO)%.o:: $(PATHS)%.c
	$(COMPILE) $(CFLAGS) $< -o $@


#Compile Unity files and store the resultant object files in object folder
$(PATHO)%.o:: $(PATHU)%.c $(PATHU)%.h
	$(COMPILE) $(CFLAGS) $< -o $@

#Create dependancy file
$(PATHD)%.d:: $(PATHT)%.c
	$(DEPEND) $@ $<

#Create directories in case they don't exist
$(PATHB):
	$(MKDIR) $(PATHB)

$(PATHD):
	$(MKDIR) $(PATHD)

$(PATHO):
	$(MKDIR) $(PATHO)

$(PATHR):
	$(MKDIR) $(PATHR)


#Clean
clean:
	$(CLEANUP) $(PATHO)*.o
	$(CLEANUP) $(PATHB)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATHR)*.txt

#Keep temporary files
.PRECIOUS: $(PATHB)Test%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATHD)%.d
.PRECIOUS: $(PATHO)%.o
.PRECIOUS: $(PATHR)%.txt
