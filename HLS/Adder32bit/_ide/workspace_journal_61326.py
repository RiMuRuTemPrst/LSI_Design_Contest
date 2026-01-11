# 2026-01-11T15:50:24.130565
import vitis

client = vitis.create_client()
client.set_workspace(path="HLS")

comp = client.get_component(name="Adder32bit_HLS_Component")
comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

comp.run(operation="IMPLEMENTATION")

