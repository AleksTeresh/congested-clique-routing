import * as d3 from 'd3'

const margin = {
    top: 80,
    right: 0,
    bottom: 10,
    left: 80
}
const width = 36
const height = 36

const setSize = 4
const n = setSize * setSize

const z = d3.scaleLinear()
    .domain([0, setSize * setSize])
    .clamp(true)

const svg = d3.select("body")
    .append("svg")
    .attr("width", width * n + margin.left + margin.right)
    .attr("height", height * n + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")")

function drawMatrix(graphHistoryPoint) {
    const n = graphHistoryPoint.nodes.length
    const nodes = []
    for (let i = 0; i < n; i++) {
        nodes.push({
            id: graphHistoryPoint.nodes[i].id,
            cells: (new Array(n))
        })
    }

    const rows = svg.selectAll(".row")
        .data(nodes)
        .enter().append("g")
        .attr("class", "row")
        .attr("transform", (_, i) => "translate(0," + (width * i) + ")")
        .each(createRow)

    rows.append("line")
        .attr("x2", width)

    rows.append("text")
        .attr("x", -6)
        .attr("y", width / 2)
        .attr("dy", ".32em")
        .attr("text-anchor", "end")
        .text((d) => d.id)

    const column = svg.selectAll(".column")
        .data(nodes)
        .enter().append("g")
        .attr("class", "column")
        .attr("transform", (_, i) => "translate(" + (width * i) + ")rotate(-90)")

    column.append("line")
        .attr("x1", -width)

    column.append("text")
        .attr("x", 6)
        .attr("y", width / 2)
        .attr("dy", ".32em")
        .attr("text-anchor", "start")
        .text((d) => d.id)

    updateMatrix(graphHistoryPoint, rows)
    return rows
}


function updateMatrix(graphHistoryPoint, row) {
    const nodes = graphHistoryPoint.nodes
    const matrix = []
    // Compute index per node.
    for (let i = 0; i < nodes.length; i++) {
        const node = {
            ...nodes[i],
            index: i,
            srcCount: 0,
            destCount: 0
        }
        matrix[i] = d3.range(n)
            .map((j) => ({x: j, y: i, z: 0}))
    }

    // Convert links to matrix count character occurrences.
    for (let i = 0; i < nodes.length; i++) {
        const node = nodes[i]
        for (let j = 0; j < node.messages.length; j++) {
            const message = node.messages[j]
            matrix[node.id][message.finalDest].z++
            nodes[node.id].srcCount++
            nodes[message.finalDest].destCount++
        }
    }

    // Precompute the orders.
    const orders = {
        id: d3.range(n).sort((a, b) =>
            { return d3.ascending(nodes[a].id, nodes[b].id) })
    }

    row.data(matrix)
        .each(updateRow)
}

function createRow(row) {
    const cell = d3.select(this)
        .selectAll(".cell")
        .data(row.cells)
        .enter().append("rect")
        .attr("class", "cell")
        .attr("x", (_, i) => width * i)
        .attr("width", width)
        .attr("height", width)
        .style("fill", 'black')
}

function updateRow(row) {
    const cell = d3.select(this)
        .selectAll(".cell")
        .data(row)
        .style("fill-opacity", (d) => z(d.z))
}

function getCppData() {
    const res = Module.random_test(setSize)
    const data = []
    for (let i = 0; i < res.size(); i++) {
        const dataPoint = res.get(i)
        const nodes = []
        for (let j = 0; j < n; j++) {
            const node = dataPoint.nodes.get(j)
            const messages = []
            for (let e = 0; e < node.messages.size(); e++) {
                const m = node.messages.get(e)  
                messages.push({
                    src: m.src,
                    finalDest: m.final_dest
                })   
            }
            nodes.push({
                id: node.id,
                messages: messages
            })
        }
        data.push({nodes})
    }
    return data
}

Module.onRuntimeInitialized = () => {
    const data = getCppData()

    let currStep = 0
    const rows = drawMatrix(data[currStep])

    const prevButton = document.getElementById('prevBtn')
    prevButton.disabled = true

    const nextButton = document.getElementById('nextBtn')

    prevButton.addEventListener(
        "click",
        () => {
            currStep--
            updateMatrix(data[currStep], rows)

            if (currStep === 0) {
                prevButton.disabled = true
            }

            nextButton.disabled = false
        });

    nextButton.addEventListener(
        "click",
        () => {
            currStep++
            updateMatrix(data[currStep], rows)

            if (currStep === 16) {
                nextButton.disabled = true
            }

            prevButton.disabled = false
        });
}
