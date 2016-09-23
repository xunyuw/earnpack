module.exports = {
    listAll,
    fn   
};

async function listAll(ctx, next) 
{
        return  ctx.body = {"id": "1235",  "name": "xouyang"};
}

async function fn(ctx, next)
{
    return ctx.body = {};
}