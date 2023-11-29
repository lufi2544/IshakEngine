# GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild prelink

ifeq ($(config),debug)
  RESCOMP = windres
  TARGETDIR = ../../Binaries/Engine/Debug
  TARGET = $(TARGETDIR)/IshakEngine
  OBJDIR = ../../Intermediate/Engine/Debug/Debug
  DEFINES += -DDEBUG_ENGINE -DLINUX
  INCLUDES += -ILaunch/Public -ICore/Public -IEcs/Public -IGame/Public -IPlatform/Public -IRenderer/Public -IEngine/Public -IIMGUI/Public -IThirdParty/Lua/Public -IThirdParty/SDL/Public -IThirdParty/SDLIMAGE/Public -IThirdParty/SolParser/Public
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -g -std=c++17 -g
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -g -std=c++17 -g
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += ../../Binaries/Engine/Debug/libEngine.so ../../Binaries/Engine/Debug/libCore.so ../../Binaries/Engine/Debug/libRenderer.so ../../Binaries/Engine/Debug/libEcs.so ../../Binaries/Engine/Debug/libLaunch.so ../../Binaries/Engine/Debug/libIMGUI.so ../../Binaries/Engine/Debug/libPlatform.so ../../Binaries/Engine/Debug/libGame.so
  LDDEPS += ../../Binaries/Engine/Debug/libEngine.so ../../Binaries/Engine/Debug/libCore.so ../../Binaries/Engine/Debug/libRenderer.so ../../Binaries/Engine/Debug/libEcs.so ../../Binaries/Engine/Debug/libLaunch.so ../../Binaries/Engine/Debug/libIMGUI.so ../../Binaries/Engine/Debug/libPlatform.so ../../Binaries/Engine/Debug/libGame.so
  ALL_LDFLAGS += $(LDFLAGS) -Wl,-rpath,'$$ORIGIN'
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: prebuild prelink $(TARGET)
	@:

endif

ifeq ($(config),release)
  RESCOMP = windres
  TARGETDIR = ../../Binaries/Engine/Release
  TARGET = $(TARGETDIR)/IshakEngine
  OBJDIR = ../../Intermediate/Engine/Release/Release
  DEFINES += -DLINUX
  INCLUDES += -ILaunch/Public -ICore/Public -IEcs/Public -IGame/Public -IPlatform/Public -IRenderer/Public -IEngine/Public -IIMGUI/Public -IThirdParty/Lua/Public -IThirdParty/SDL/Public -IThirdParty/SDLIMAGE/Public -IThirdParty/SolParser/Public
  FORCE_INCLUDE +=
  ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
  ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O2 -std=c++17 -g
  ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O2 -std=c++17 -g
  ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
  LIBS += ../../Binaries/Engine/Release/libEngine.so ../../Binaries/Engine/Release/libCore.so ../../Binaries/Engine/Release/libRenderer.so ../../Binaries/Engine/Release/libEcs.so ../../Binaries/Engine/Release/libLaunch.so ../../Binaries/Engine/Release/libIMGUI.so ../../Binaries/Engine/Release/libPlatform.so ../../Binaries/Engine/Release/libGame.so
  LDDEPS += ../../Binaries/Engine/Release/libEngine.so ../../Binaries/Engine/Release/libCore.so ../../Binaries/Engine/Release/libRenderer.so ../../Binaries/Engine/Release/libEcs.so ../../Binaries/Engine/Release/libLaunch.so ../../Binaries/Engine/Release/libIMGUI.so ../../Binaries/Engine/Release/libPlatform.so ../../Binaries/Engine/Release/libGame.so
  ALL_LDFLAGS += $(LDFLAGS) -Wl,-rpath,'$$ORIGIN' -s
  LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
  define PREBUILDCMDS
  endef
  define PRELINKCMDS
  endef
  define POSTBUILDCMDS
  endef
all: prebuild prelink $(TARGET)
	@:

endif

OBJECTS := \

RESOURCES := \

CUSTOMFILES := \

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

$(TARGET): $(GCH) ${CUSTOMFILES} $(OBJECTS) $(LDDEPS) $(RESOURCES) | $(TARGETDIR)
	@echo Linking IshakEngine
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(CUSTOMFILES): | $(OBJDIR)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning IshakEngine
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild:
	$(PREBUILDCMDS)

prelink:
	$(PRELINKCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) $(PCH) | $(OBJDIR)
$(GCH): $(PCH) | $(OBJDIR)
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
else
$(OBJECTS): | $(OBJDIR)
endif


-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(OBJDIR)/$(notdir $(PCH)).d
endif