# 2025-12-03T20:35:38.812471
import vitis

client = vitis.create_client()
client.set_workspace(path="Lab4")

comp = client.create_hls_component(name = "hls_component_lab4",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="hls_component_lab4")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

status = comp.remove_cfg_files(cfg_files=["hls_config.cfg"])

status = comp.add_cfg_files(cfg_files=["hls_config.cfg"])

vitis.dispose()

