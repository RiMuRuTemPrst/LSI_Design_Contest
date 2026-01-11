# 2026-01-11T16:08:39.949077
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS")

vitis.dispose()

