include $(TOPDIR)/rules.mk

# Name, version and release number
# The name and version of your package are used to define the variable to point to the build directory of your package: $(PKG_BUILD_DIR)
PKG_NAME:=demo1
PKG_VERSION:=1.0
PKG_RELEASE:=1

# Source settings (i.e. where to find the source codes)
# This is a custom variable, used below
SOURCE_DIR:=./src

include $(INCLUDE_DIR)/package.mk
# PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

# Package definition; instructs on how and where our package will appear in the overall configuration menu ('make menuconfig')
define Package/$(PKG_NAME)
  SECTION:=skps
  CATEGORY:=skps
  TITLE:=skpsprojekt
endef

# Package description; a more verbose description on what our package does
define Package/$(PKG_NAME)/description
  Project for SKPS leds steered by joysticks using ADC
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
	$(Build/Patch)
endef



# Package build instructions; invoke the target-specific compiler to first compile the source file, and then to link the file into the final executable
define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/server.o -c $(PKG_BUILD_DIR)/server.c
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/mcp3424.o -c $(PKG_BUILD_DIR)/mcp3424.c
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/slave.o -c $(PKG_BUILD_DIR)/slave.c
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/master.o -c $(PKG_BUILD_DIR)/master.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/master $(PKG_BUILD_DIR)/master.o $(PKG_BUILD_DIR)/server.o $(PKG_BUILD_DIR)/mcp3424.o
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/slave $(PKG_BUILD_DIR)/slave.o
endef



# Package install instructions; create a directory inside the package to hold our executable, and then copy the executable we built previously into the folder
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/master $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/slave $(1)/usr/bin
endef

# This command is always the last, it uses the definitions and variables we give above in order to get the job done
$(eval $(call BuildPackage,$(PKG_NAME)))
