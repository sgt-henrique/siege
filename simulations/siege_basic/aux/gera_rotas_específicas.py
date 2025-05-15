import xml.etree.ElementTree as ET

def carregar_rede(caminho_arquivo):
    """
    Carrega o arquivo .net.xml e retorna os elementos das vias (edges).
    """
    tree = ET.parse(caminho_arquivo)
    root = tree.getroot()
    
    # Encontrar todas as vias no arquivo .net.xml
    edges = {}
    for edge in root.findall(".//edge"):
        edge_id = edge.get("id")
        max_speed = float(edge.get("speed", "0"))
        edges[edge_id] = {
            "id": edge_id,
            "max_speed": max_speed
        }
    
    return edges

def gerar_rota(veiculo_id, edges):
    """
    Gera uma rota para o veículo, priorizando vias com maior limite de velocidade.
    """
    # Ordenar as vias por velocidade máxima
    vias_ordenadas = sorted(edges.values(), key=lambda e: e["max_speed"], reverse=True)
    
    # Selecionar uma sequência de vias (por exemplo, 5 vias)
    rota = [via["id"] for via in vias_ordenadas[:5]]
    
    # Exemplo de saída
    return f"""
    <vehicle id="{veiculo_id}" type="passenger" depart="0" departLane="best" departSpeed="max">
        <route edges="{" ".join(rota)}"/>
    </vehicle>
    """

def gerar_veiculos(caminho_arquivo, num_veiculos=5):
    """
    Gera veículos automaticamente com base na rede fornecida.
    """
    edges = carregar_rede(caminho_arquivo)
    
    veiculos_xml = ""
    for i in range(num_veiculos):
        veiculo_id = f"veiculo_{i+1}"
        veiculos_xml += gerar_rota(veiculo_id, edges)
    
    # Adicionar o tempo de permanência de 600 segundos e a possibilidade de estrapolar a velocidade
    veiculos_xml = f"""
    <routes xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://sumo.dlr.de/xsd/routes_file.xsd">
        <vType id="passenger" vClass="passenger" maxSpeed="50"/>
        {veiculos_xml}
    </routes>
    """
    
    return veiculos_xml

# Caminho para o arquivo .net.xml
caminho_arquivo = "grid3000.net.xml"

# Gerar veículos com base na rede
veiculos_gerados = gerar_veiculos(caminho_arquivo)

# Salvar as rotas geradas em um arquivo .rou.xml
with open("rotas_geradas.rou.xml", "w") as f:
    f.write(veiculos_gerados)

print("Arquivo de rotas gerado com sucesso!")

