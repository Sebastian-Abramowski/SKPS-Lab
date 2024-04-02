include $(TOPDIR)/rules.mk

# Name, version and release number
# The name and version of your package are used to define the variable to point to the build directory of your package: $(PKG_BUILD_DIR)
PKG_NAME:=worms
PKG_VERSION:=1.0
PKG_RELEASE:=1

# Source settings (i.e. where to find the source codes)
# This is a custom variable, used below
SOURCE_DIR:=./src

include $(INCLUDE_DIR)/package.mk
# PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

# Package definition; instructs on how and where our package will appear in the overall configuration menu ('make menuconfig')
define Package/$(PKG_NAME)
  SECTION:=examples
  CATEGORY:=Examples
  TITLE:=Worms
  DEPENDS:=+libncurses
endef

# Package description; a more verbose description on what our package does
define Package/$(PKG_NAME)/description
  Worms game.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
	$(Build/Patch)
endef



# Package build instructions; invoke the target-specific compiler to first compile the source file, and then to link the file into the final executable
define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/worms.o -c $(PKG_BUILD_DIR)/worms.c
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/helper.o -c $(PKG_BUILD_DIR)/helper.c
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/main.o -c $(PKG_BUILD_DIR)/main.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/worms $(PKG_BUILD_DIR)/main.o $(PKG_BUILD_DIR)/helper.o $(PKG_BUILD_DIR)/worms.o -lncurses
endef



# Package install instructions; create a directory inside the package to hold our executable, and then copy the executable we built previously into the folder
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/worms $(1)/usr/bin
endef

# This command is always the last, it uses the definitions and variables we give above in order to get the job done
$(eval $(call BuildPackage,$(PKG_NAME)))

