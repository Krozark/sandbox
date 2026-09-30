#!/usr/bin/env bash
# Downloads the newest artifact <name> built by the CI of the Krozark/vosk-api fork (master).
# Temporary bridge until the fork publishes GitHub releases: then this becomes `gh release download`.
#
# Usage: ci/fetch_vosk_binaries.sh <artifact_name> <destination_dir>
# Artifacts of the fork: libvosk-ios-xcframework, vosk-android, vosk-linux-x86_64, vosk-windows-x64, ...
# Needs GH_TOKEN (the workflow's GITHUB_TOKEN is enough: the fork is public).
set -euo pipefail

NAME="${1:?usage: $0 <artifact_name> <destination_dir>}"
DEST="${2:?usage: $0 <artifact_name> <destination_dir>}"
REPO=Krozark/vosk-api

run_id="$(gh api "repos/$REPO/actions/artifacts?name=$NAME&per_page=1" --jq '.artifacts[0].workflow_run.id')"
if [ -z "$run_id" ] || [ "$run_id" = "null" ]; then
    echo "::error::no artifact named '$NAME' in $REPO (build not green yet, or expired)"
    exit 1
fi
echo "Downloading '$NAME' from $REPO run $run_id"
gh run download "$run_id" --repo "$REPO" --name "$NAME" --dir "$DEST"
