# 2025-12-01T13:39:36.252343
import vitis

client = vitis.create_client()
client.set_workspace(path="Lab3")

comp = client.get_component(name="hls_component_dct")
comp.run(operation="CO_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="CO_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="CO_SIMULATION")

comp.run(operation="CO_SIMULATION")

comp.run(operation="C_SIMULATION")

comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp = client.create_hls_component(name = "hls_component_lab3_2",cfg_file = ["/home/rimurutempest/Code/LSI_Design_Contest/HLS_DesignFlow/Lab3/hls_component_dct/hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="hls_component_lab3_2")
comp.run(operation="SYNTHESIS")

vitis.dispose()

vitis.dispose()

