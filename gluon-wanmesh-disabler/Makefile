include $(TOPDIR)/rules.mk

PKG_NAME:=gluon-wanmesh-disabler
PKG_VERSION:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/gluon-wanmesh-disabler
  SECTION:=gluon
  CATEGORY:=Workaround
  TITLE:=Disable Mesh-on-WAN when no mesh partners are found on WAN
  DEPENDS:=+gluon-core +micrond
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/gluon-wanmesh-disabler/install
	$(CP) ./files/* $(1)/
endef

$(eval $(call BuildPackage,gluon-wanmesh-disabler))
