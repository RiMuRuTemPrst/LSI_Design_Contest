# 2025-11-29T21:37:42.820967
import vitis

client = vitis.create_client()
client.set_workspace(path="Lab2")

comp = client.create_hls_component(name = "Lab2_HLS_Componet",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="Lab2_HLS_Componet")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

status = comp.remove_cfg_files(cfg_files=["hls_config.cfg"])

status = comp.add_cfg_files(cfg_files=["hls_config.cfg"])

comp.run(operation="C_SIMULATION")

vitis.dispose()

