include $(TOPDIR)/rules.mk

PKG_NAME:=gluon-vfn-cleanup
PKG_VERSION:=1
PKG_RELEASE:=$(GLUON_BRANCH)

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(GLUONDIR)/include/package.mk


define Package/$(PKG_BUILD_DIR)
  SECTION:=gluon
  CATEGORY:=Gluon
  DEPENDS:=+gluon-core
  TITLE:=Removes old settings from previous firmware versions
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(call GluonSrcDiet,./luasrc,$(PKG_BUILD_DIR)/luadest/)
endef

define Package/$(PKG_BUILD_DIR)/install
	$(CP) $(PKG_BUILD_DIR)/luadest/* $(1)/
endef

define Package/$(PKG_BUILD_DIR)/postinst
endef

$(eval $(call BuildPackage,$(PKG_BUILD_DIR)))
