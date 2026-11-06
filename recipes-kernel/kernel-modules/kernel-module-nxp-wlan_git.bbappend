FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# The CIP patches in recipes-kernel/linux/linux-imx/cip/ bring the kernel to
# 6.12.101 or later, where the stable series backported the 6.13 change that
# added a struct net_device argument to cfg80211_ops.set_monitor_channel.
# The NXP driver expects the new prototype only from 6.13, so without this
# patch moal_sta_cfg80211.c fails with -Wincompatible-pointer-types.
SRC_URI:append:ecu150v2 = " file://0001-mlinux-Use-the-new-set_monitor_channel-prototype-fro.patch"
