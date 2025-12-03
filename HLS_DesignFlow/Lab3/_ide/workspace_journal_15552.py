# 2025-12-01T13:25:51.632124
import vitis

client = vitis.create_client()
client.set_workspace(path="Lab3")

comp = client.get_component(name="hls_component_dct")
comp.run(operation="CO_SIMULATION")

comp.run(operation="CO_SIMULATION")

comp.run(operation="CO_SIMULATION")

vitis.dispose()

