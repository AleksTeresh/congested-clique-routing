import * as d3 from 'd3'

const margin = {top: 80, right: 0, bottom: 10, left: 80}
const width = 36
const height = 36

const setSize = 4

const x = d3.scaleBand().domain([]).range([0, width]),
    z = d3.scaleLinear().domain([0, setSize * setSize]).clamp(true);
    // c = d3.scale.category10().domain(d3.range(10))

const svg = d3.select("body").append("svg")
    .attr("width", width * setSize * setSize + margin.left + margin.right)
    .attr("height", height * setSize * setSize + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")")

const drawMatrix = (graphHistoryPoint) => {
    const matrix = []
    const nodes = graphHistoryPoint.nodes
    const n = nodes.size()

    // Compute index per node.
    for (let i = 0; i < nodes.size(); i++) {
        const node = nodes.get(i)
        node.index = i
        node.srcCount = 0
        node.destCount = 0
        matrix[i] = d3.range(n).map((j) => { return {x: j, y: i, z: 0} })
    }

    // Convert links to matrix count character occurrences.
    for (let i = 0; i < nodes.size(); i++) {
        const node = nodes.get(i)
        for (let j = 0; j < node.messages.size(); j++) {
            const message = node.messages.get(j)
            matrix[node.id][message.final_dest].z++
            nodes.get(node.id).srcCount++
            nodes.get(message.final_dest).destCount++
        }
    }

    // Precompute the orders.
    const orders = {
        id: d3.range(n).sort((a, b) => { return d3.ascending(nodes.get(a).id, nodes.get(b).id) }),
        srcCount: d3.range(n).sort((a, b) => { return nodes.get(b).srcCount - nodes.get(a).srcCount }),
        destCount: d3.range(n).sort((a, b) => { return nodes.get(b).destCount - nodes.get(a).destCount })
    }

    // The default sort order.
    x.domain(orders.id)

    // svg.append("rect")
    //     .attr("class", "background")
    //     .attr("width", width)
    //     .attr("height", height)

    const row = svg.selectAll(".row")
        .data(matrix)
        .enter().append("g")
        .attr("class", "row")
        .attr("transform", function(d, i) { return "translate(0," + (width * i) + ")" })
        .each(processRow)

    row.append("line")
        .attr("x2", width)

    row.append("text")
        .attr("x", -6)
        .attr("y", width / 2)
        .attr("dy", ".32em")
        .attr("text-anchor", "end")
        .text(function(d, i) { return nodes.get(i).id })

    const column = svg.selectAll(".column")
        .data(matrix)
        .enter().append("g")
        .attr("class", "column")
        .attr("transform", function(d, i) { return "translate(" + (width * i) + ")rotate(-90)" })

    column.append("line")
        .attr("x1", -width)

    column.append("text")
        .attr("x", 6)
        .attr("y", width / 2)
        .attr("dy", ".32em")
        .attr("text-anchor", "start")
        .text(function(d, i) { return nodes.get(i).id })

    function processRow (row) {
        const cell = d3.select(this).selectAll(".cell")
            .data(row)
            .enter().append("rect")
            .attr("class", "cell")
            .attr("x", function(d) { return (width * d.x) })
            .attr("width", width)
            .attr("height", width)
            .style("fill-opacity", function(d) { return z(d.z) })
            .style("fill", 'black')
        // .on("mouseover", mouseover)
        // .on("mouseout", mouseout)
    }

    return {
        updateMatrix: (graphHistoryPoint) => {
            const nodes = graphHistoryPoint.nodes
            // Compute index per node.
            for (let i = 0; i < nodes.size(); i++) {
                const node = nodes.get(i)
                node.index = i
                node.srcCount = 0
                node.destCount = 0
                matrix[i] = d3.range(n).map((j) => { return {x: j, y: i, z: 0} })
            }

            // Convert links to matrix count character occurrences.
            for (let i = 0; i < nodes.size(); i++) {
                const node = nodes.get(i)
                for (let j = 0; j < node.messages.size(); j++) {
                    const message = node.messages.get(j)
                    matrix[node.id][message.final_dest].z++
                    nodes.get(node.id).srcCount++
                    nodes.get(message.final_dest).destCount++
                }
            }

            row.data(matrix)
                .attr("class", "row")
                .attr("transform", function(d, i) { return "translate(0," + (width * i) + ")" })
                .each(processRow1)

            function processRow1 (row) {
                console.log(row)
                const cell = d3.select(this).selectAll(".cell")
                    .data(row)
                    // .enter().append("rect")
                    .attr("class", "cell")
                    .attr("x", function(d) { return (width * d.x) })
                    .attr("width", width)
                    .attr("height", width)
                    .style("fill-opacity", function(d) { return z(d.z) })
                    .style("fill", 'black')
                // .on("mouseover", mouseover)
                // .on("mouseout", mouseout)
            }
        }
    }
}

Module.onRuntimeInitialized = () => {
    const res = Module.random_test(setSize)

    let currStep = 0;
    const t = drawMatrix(res.get(currStep))

    const prevButton = document.getElementById('prevBtn')
    prevButton.disabled = true

    const nextButton = document.getElementById('nextBtn')

    prevButton.addEventListener(
        "click",
        () => {
            currStep--;
            t.updateMatrix(res.get(currStep))

            if (currStep === 0) {
                prevButton.disabled = true
            }

            nextButton.disabled = false
        });

    nextButton.addEventListener(
        "click",
        () => {
            currStep++;
            t.updateMatrix(res.get(currStep))

            if (currStep === 16) {
                nextButton.disabled = true
            }

            prevButton.disabled = false
        });
}