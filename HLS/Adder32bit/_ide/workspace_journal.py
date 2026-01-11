# 2026-01-11T21:59:19.228985
import vitis

client = vitis.create_client()
client.set_workspace(path="Adder32bit")

vitis.dispose()

