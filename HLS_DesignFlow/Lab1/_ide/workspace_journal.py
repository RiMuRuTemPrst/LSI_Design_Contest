# 2025-11-29T00:12:25.293091
import vitis

client = vitis.create_client()
client.set_workspace(path="Lab1")

comp = client.get_component(name="matrixmul")
comp.run(operation="SYNTHESIS")

comp.run(operation="CO_SIMULATION")

comp.run(operation="CO_SIMULATION")

comp.run(operation="CO_SIMULATION")

comp.run(operation="PACKAGE")

comp.run(operation="IMPLEMENTATION")

vitis.dispose()

