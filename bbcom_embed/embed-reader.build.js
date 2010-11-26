{
    baseUrl: "../../lingwo_dictionary/js/require",
    paths: {
        lingwo_dictionary: "..",
        bibliobird: "../../../lingwoorg/js"
    },
    out: "embed-reader.uncompressed.js",
    optimize: "none",

    include: ["jquery-stubs","bibliobird/Embed"],
    skipModuleInsertion: true,
    pragmas: {
        requireExcludeModify: true,
        requireExcludePlugin: true,
        requireExcludeContext: true
    }
}
