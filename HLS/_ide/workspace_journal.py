# 2026-01-11T16:16:05.684597
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS")

comp = client.get_component(name="Adder32bit_HLS_Component")
comp.run(operation="PACKAGE")

vitis.dispose()

