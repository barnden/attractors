// // ISITALLDARLI
// //           "CVQKGHQTPHTE"
// // EMXOWLUPOYMI
// // HSSDRNKOMUDP
// // QOXKHESSLDDA
// let params = "MRGCSSWOIQWC"
//         //ISITALLDCRLI
//         //MADSYELKKTDN
//         //RHRBWINGOFCS
//         //WARIETKXHWCK
//         //MXHDDWTKUBAO
//         //MRGCSSWOIQWC
//         //JTLNPTNWKJDB
//         //CHXLMPJDVICS

//         //BIWGBJBWUXXV NO
//         //LUKIYUDIWTGY NO
//         //VDQPOKMELIFS NO

const params = "CVQKGHQTPHTE"

document.getElementById("desc").innerHTML = params

let qaz = (init, get_x, get_y, next, color) => {
    let canvas, ctx, img, heap
    let points = []

    function setup() {
        canvas = document.getElementById("display")
        ctx = canvas.getContext("2d")

        canvas.width = window.innerWidth
        canvas.height = window.innerHeight

        img = ctx.createImageData(canvas.width, canvas.height)
        data = img.data

        ctx.imageSmoothingEnabled = true

        let consts = new Float32Array(12);

        for (let i in params)
            consts[i] = parseFloat((-1.2 + 0.1 * (params.charCodeAt(i) - 65)).toFixed(1))

        heap = Module._malloc(12 * consts.BYTES_PER_ELEMENT)
        Module.HEAPF32.set(consts, heap >> 2)

        init(
            window.innerWidth,
            window.innerHeight,
            600.0,
            heap,
            + .65,
            + .45
        )
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
        next()
        console.log(get_x(), get_y())
    }

    function loop() {
        let iterations = 0
        let last = 0

        function _loop() {
            let i = 0

            for (let _ = 0; _ < 1; _++) {
                // points.push([get_x(), get_y()])
                next()
            }

            // for (let i = 0; i < 200; i++) {
            //     if (typeof points[i] != "object") continue

            //     let [x, y] = points[i + last]
            //     if (x == -1 || y == -1) continue

            //     let pos = (x + y * canvas.width) * 4
            //     let col = color(x, y)

            //     col = 0xFFFFFF

            //     data[pos]     = col >> 16
            //     data[pos + 1] = (col >> 8) & 0xFF
            //     data[pos + 2] = col & 0xFF
            //     data[pos + 3] = 0xFF
            // }

            last += i

            if (++iterations <= 10) setTimeout(_loop(), 15)
            else Module._free(heap)

            ctx.putImageData(img, 0, 0)
        }

        _loop()
    }

    setup()
}

Module.onRuntimeInitialized = () => {
    qaz(
        Module._init,
        Module._x,
        Module._y,
        Module._next,
        Module._color
    )
}