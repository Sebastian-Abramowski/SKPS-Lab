include $(TOPDIR)/rules.mk

# Name, version and release number
# The name and version of your package are used to define the variable to point to the build directory of your package: $(PKG_BUILD_DIR)
PKG_NAME:=buggy
PKG_VERSION:=1.0
PKG_RELEASE:=1

# Source settings (i.e. where to find the source codes)
# This is a custom variable, used below
SOURCE_DIR:=./src

include $(INCLUDE_DIR)/package.mk

# Package definition; instructs on how and where our package will appear in the overall configuration menu ('make menuconfig')
define Package/$(PKG_NAME)
  SECTION:=examples
  CATEGORY:=Examples
  TITLE:=Buggy
endef

# Package description; a more verbose description on what our package does
define Package/$(PKG_NAME)/description
  Three programs with different bugs.
endef

# Package preparation instructions; create the build directory and copy the source code.
# The last command is necessary to ensure our preparation instructions remain compatible with the patching system.
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
	$(Build/Patch)
endef

# Package build instructions; invoke the target-specific compiler to first compile the source file, and then to link the file into the final executable
define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -ggdb -o $(PKG_BUILD_DIR)/bug1.o -c $(PKG_BUILD_DIR)/bug1.c
	$(TARGET_CC) $(TARGET_CFLAGS) -ggdb -o $(PKG_BUILD_DIR)/bug2.o -c $(PKG_BUILD_DIR)/bug2.c
	$(TARGET_CC) $(TARGET_CFLAGS) -ggdb -o $(PKG_BUILD_DIR)/bug3.o -c $(PKG_BUILD_DIR)/bug3.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/bug1 $(PKG_BUILD_DIR)/bug1.o
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/bug2 $(PKG_BUILD_DIR)/bug2.o
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/bug3 $(PKG_BUILD_DIR)/bug3.o

endef

# Package install instructions; create a directory inside the package to hold our executable, and then copy the executable we built previously into the folder
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bug1 $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bug2 $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bug3 $(1)/usr/bin
endef

# This command is always the last, it uses the definitions and variables we give above in order to get the job done
$(eval $(call BuildPackage,$(PKG_NAME)))

