#!/bin/bash

IMG=noble-server-cloudimg-amd64.img
IMG_ORG=${IMG}.org

# Step 1: Download image if not already present
if [ ! -f "${IMG_ORG}" ]; then
    echo "[INFO] Downloading original image..."
    wget https://cloud-images.ubuntu.com/noble/current/${IMG} -O "${IMG_ORG}" || {
        echo "[ERROR] Failed to download image." >&2
        exit 1
    }
fi

# Always start from original image
cp "${IMG_ORG}" "${IMG}"

# Step 2: Create user-data
cp user-data.first_time user-data

# Step 3: Generate cloud-init ISO
genisoimage -output cloud-init.iso -volid cidata -joliet -rock user-data meta-data

# Step 4: Resize kernel image
qemu-img resize "${IMG}" +4G

# Step 5: Boot VM with QEMU
qemu-system-x86_64 \
    -smp 2 -m 2G -nographic \
    -hda "./${IMG}" \
    -device e1000,netdev=net0 -netdev user,id=net0,hostfwd=tcp::2200-:22 \
    -cdrom ./cloud-init.iso

