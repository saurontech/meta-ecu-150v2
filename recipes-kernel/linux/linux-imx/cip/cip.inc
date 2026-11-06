require cip-series.inc

SRC_URI:append:ecu150v2 = " ${@' '.join('file://cip/%s/' % b for b in d.getVar('CIP_BATCHES').split())}"

do_patch:prepend:ecu150v2() {
    cd ${S}
    check_git_config
    # Disable automatic gc in the kernel source repo. Tens of thousands of CIP
    # commits leave many loose objects, and the plain "git am" that kgit-s2q
    # uses for the SRC_URI patches would then start a background gc, which
    # holds HEAD.lock while the next patch is being committed.
    git config gc.auto 0
    git config maintenance.auto false
    if [ "${KERNEL_DEBUG_TIMESTAMPS}" != "1" ]; then
        reproducible_git_committer_author
    fi
    for batch in ${CIP_BATCHES}; do
        dir=${UNPACKDIR}/cip/$batch
        if ! ls $dir | grep '\.patch$' | sort -V | sed "s|^|$dir/|" \
            | xargs -d '\n' sh -c 'git -c gc.auto=0 am --no-3way -q "$@" || exit 255' sh; then
            failed=$(git am --show-current-patch=raw 2>/dev/null | grep -m1 '^Subject:' || true)
            git am --abort || true
            bbfatal "CIP $batch: git am failed at: $failed"
        fi
    done
}