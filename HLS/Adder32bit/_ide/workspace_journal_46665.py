# 2026-01-11T15:42:53.859535
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS")

comp = client.create_hls_component(name = "Adder32bit_HLS_Component",cfg_file = ["hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="Adder32bit_HLS_Component")
comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

vitis.dispose()

