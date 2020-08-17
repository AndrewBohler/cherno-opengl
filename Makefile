# use \\ because windows sucks lol
OPENGLDIR := C:\OpenGL
GLWFDIR   := C:\ProgramData\chocolatey\lib\glfw3\tools\glfw-3.0.4.bin.WIN32
PROJECTDIR := $(shell echo %CD%)

define \n


endef

OBJDIR := obj
BINDIR := bin
SRCDIR := src

SRCDIRS := src $(foreach path,$(shell dir src /AD /B /S),$(subst $(PROJECTDIR)\,,$(path)))

EXENAME := cherno-opengl.exe
HEADERS := $(wildcard $(SRCDIR)/*.h)
CPPSRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(subst $(SRCDIR),$(OBJDIR),$(CPPSRCS:.cpp=.o))

# g++ -L flags
externalLibs := \
$(OPENGLDIR)\lib\release\Win32 \

# -L
LDFLAGS := $(addprefix -L,$(externalLibs))
LDFLAGS += -Ldep/lib/win32

INCLUDEPATHS := \
$(SRCDIRS) \
dep\include \
src\vendor \
$(OPENGLDIR)\include

IFLAGS = $(addprefix -I", $(addsuffix ", $(INCLUDEPATHS)))

# g++ -l flags, order matters!
LDLIBS :=\
-lglfw3 \
-l:libglfw3dll.a \
-lgdi32 \
-lglu32 \
-lglew32 \
-lopengl32

CPPFLAGS := -g -Wall -DGLFW_DLL

DEPFILES := $(foreach dep,$(wildcard $(OBJDIR)/*.d),$(dep))
include $(DEPFILES)

.PHONY: all compile build vendor info dependencies clean run \
clean-all clean-bin clean-obj clean-dep \

all: dependencies include build
	@echo $^ completed

# compile thirdparty sources (forced)
vendor:
	@echo compiling vendor source files...\
	$(foreach subdir,$(wildcard src/vendor/*/),\
		$(foreach source,$(wildcard $(subdir)*.cpp),\
			& echo   $(source) -^> $(OBJDIR)/$(notdir $(source:.cpp=.o)) & \
			g++ $(CPPFLAGS) -c $(source) -o $(OBJDIR)/$(notdir $(source:.cpp=.o)) \
			$(LDFLAGS) $(IFLAGS) $(LDLIBS) \
		)\
	)

build: $(BINDIR)/$(EXENAME)
	@echo build finished

# force all non-third-party objects to recompile
force-compile:
	@echo compiling objects \
	$(foreach file,$(OBJECTS), & echo $(subst $(OBJDIR),$(SRCDIR),$(file:.o=.cpp)) -^>  $(file) &\
		g++ $(CPPFLAGS) -c $(subst $(OBJDIR),$(SRCDIR),$(file:.o=.cpp)) -o $(file) $(LDFLAGS) $(IFLAGS) $(LDLIBS)\
	)

dependencies:
	@echo Building dependency files...\
	$(foreach src,$(CPPSRCS), \
	& g++ -MM $(src) \
	-MT"$(OBJDIR)/$(notdir $(src:.cpp=.o))" \
	-o $(subst $(SRCDIR),$(OBJDIR),$(src:.cpp=.d))) \
	$(LDFLAGS) $(IFLAGS) $(LDLIBS)

clean: clean-obj
	@echo $^ complete

clean-all: clean-obj clean-bin clean-dep
	@echo $^ completed

clean-obj:
	del $(subst /,\,$(OBJDIR)\*.o)

clean-bin:
	del $(subst /,\,$(BINDIR)\*.exe)
	del $(subst /,\,$(BINDIR)\*.dll)

clean-dep:
	del $(subst /,\,$(OBJDIR)\*.d)

# build exe
$(BINDIR)/$(EXENAME): $(OBJECTS) | $(OBJDIR) $(BINDIR) copy_dlls
	@echo building $(EXENAME)
	g++ $(CPPFLAGS) -o $@ $(foreach obj,$(wildcard $(OBJDIR)/*.o),$(obj)) $(LDFLAGS) $(IFLAGS) $(LDLIBS)

# create object directory
$(OBJDIR):
	mkdir $(OBJDIR)

# create binary directory
$(BINDIR):
	mkdir $(BINDIR)

obj/%.o:
	@echo $< -^> $@ & g++ $(CPPFLAGS) -c $< -o $@ $(LDFLAGS) $(IFLAGS) $(LDLIBS)

run:
	@$(subst /,\,$(BINDIR)\$(EXENAME))

copy_dlls:
	xcopy dep\lib\win32\ $(BINDIR)\ /y /d

info:
	@echo *info not configured*
