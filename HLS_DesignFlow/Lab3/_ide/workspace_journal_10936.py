# 2025-12-01T12:39:48.962566
import vitis

client = vitis.create_client()
client.set_workspace(path="Lab3")

comp = client.create_hls_component(name = "hls_component_dct",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="hls_component_dct")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

