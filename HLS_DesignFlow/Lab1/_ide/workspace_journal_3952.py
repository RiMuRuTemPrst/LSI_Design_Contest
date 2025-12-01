# 2025-11-28T23:02:39.861487
import vitis

client = vitis.create_client()
client.set_workspace(path="Lab1")

comp = client.create_hls_component(name = "matrixmul",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="matrixmul")
comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION_DEBUG")

vitis.dispose()

