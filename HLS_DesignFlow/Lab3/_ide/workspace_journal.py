# 2025-12-02T13:35:30.936303
import vitis

client = vitis.create_client()
client.set_workspace(path="Lab3")

comp = client.get_component(name="hls_component_dct")
comp.run(operation="SYNTHESIS")

comp = client.create_hls_component(name = "hls_component_lab3_3",cfg_file = ["/home/rimurutempest/Code/LSI_Design_Contest/HLS_DesignFlow/Lab3/hls_component_dct/hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="hls_component_lab3_3")
comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp.run(operation="SYNTHESIS")

comp = client.create_hls_component(name = "hls_component_lab3_4",cfg_file = ["/home/rimurutempest/Code/LSI_Design_Contest/HLS_DesignFlow/Lab3/hls_component_dct/hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="hls_component_lab3_4")
comp.run(operation="SYNTHESIS")

comp = client.create_hls_component(name = "hls_component_lab3_5",cfg_file = ["/home/rimurutempest/Code/LSI_Design_Contest/HLS_DesignFlow/Lab3/hls_component_dct/hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="hls_component_lab3_5")
comp.run(operation="SYNTHESIS")

comp = client.create_hls_component(name = "hls_component_lab3_6",cfg_file = ["/home/rimurutempest/Code/LSI_Design_Contest/HLS_DesignFlow/Lab3/hls_component_dct/hls_config.cfg"],template = "empty_hls_component")

comp = client.get_component(name="hls_component_lab3_6")
comp.run(operation="SYNTHESIS")

vitis.dispose()

